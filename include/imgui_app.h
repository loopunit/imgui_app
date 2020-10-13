#pragma once

#include <functional>
#include <atomic>
#include <array>

namespace imgui_app
{
	template<typename E>
	constexpr auto underlying_cast(E e) -> typename std::underlying_type<E>::type
	{
		return static_cast<typename std::underlying_type<E>::type>(e);
	}
} // namespace imgui_app

namespace imgui_app
{
	namespace details
	{
		template<typename T>
		class static_root_singleton
		{
		public:
			T* operator->()
			{
				return s_instance;
			}

			const T* operator->() const
			{
				return s_instance;
			}

			T& operator*()
			{
				return *s_instance;
			}

			const T& operator*() const
			{
				return *s_instance;
			}

			static_root_singleton()
			{
				static bool static_init = []() -> bool {
					s_instance = new (&s_instance_memory[0]) T();
					std::atexit(destroy);
					return true;
				}();
			}

		protected:
			static void destroy()
			{
				if (s_instance)
				{
					s_instance->~T();
					s_instance = nullptr;
				}
			}

		private:
			static inline uint64_t s_instance_memory[1 + (sizeof(T) / sizeof(uint64_t))];
			static inline T*	   s_instance = nullptr;
		};

		template<size_t POOL_COUNT>
		class singleton_cleanup_list
		{
		public:
			using cleanup_func = std::function<void()>;

			void push(cleanup_func f)
			{
				s_cleanup_funcs[s_cleanup_index.fetch_add(1)] = f;
			}

			~singleton_cleanup_list()
			{
				for (auto i = s_cleanup_index.load(); i > 0; --i)
				{
					s_cleanup_funcs[i - 1]();
				}
				s_cleanup_index.store(0);
			}

		private:
			std::array<cleanup_func, POOL_COUNT> s_cleanup_funcs;
			std::atomic_size_t					 s_cleanup_index = 0;
		};

		using singleton_cleanup_root = details::static_root_singleton<details::singleton_cleanup_list<1024>>;

		template<typename... T_DEPS>
		struct singleton_dependencies
		{
		private:
			template<typename T_ARG, typename... T_ARGS>
			static inline void update_dependencies_impl()
			{
				const typename T_ARG::type* t = T_ARG().get();
				if constexpr (sizeof...(T_ARGS) > 0)
				{
					update_dependencies_impl<T_ARGS...>();
				}
			}

		public:
			static inline void update()
			{
				if constexpr (sizeof...(T_DEPS) > 0)
				{
					update_dependencies_impl<T_DEPS...>();
				}
			}
		};

		template<typename T, typename... T_DEPS>
		struct singleton_factory
		{
			static inline T* create()
			{
				if constexpr (sizeof...(T_DEPS) > 0)
				{
					singleton_dependencies<T_DEPS...>::update();
				}
				return new T();
			}
		};

		template<typename T>
		struct virtual_singleton_factory
		{
			static T* create();
		};
	} // namespace details

#define imgui_app_DEFINE_VIRTUAL_SINGLETON(T, T_DERIVED)                                                                                                                           \
	namespace imgui_app                                                                                                                                                            \
	{                                                                                                                                                                              \
		namespace details                                                                                                                                                          \
		{                                                                                                                                                                          \
			template<>                                                                                                                                                             \
			T* virtual_singleton_factory<T>::create()                                                                                                                              \
			{                                                                                                                                                                      \
				return new T_DERIVED;                                                                                                                                              \
			}                                                                                                                                                                      \
		}                                                                                                                                                                          \
	}                                                                                                                                                                              \
	/**/

#define imgui_app_DEFINE_VIRTUAL_SINGLETON_DEPS(T, T_DERIVED, ...)                                                                                                                 \
	namespace imgui_app                                                                                                                                                            \
	{                                                                                                                                                                              \
		namespace details                                                                                                                                                          \
		{                                                                                                                                                                          \
			template<>                                                                                                                                                             \
			T* virtual_singleton_factory<T>::create()                                                                                                                              \
			{                                                                                                                                                                      \
				singleton_dependencies<__VA_ARGS__>::update();                                                                                                                     \
				return new T_DERIVED;                                                                                                                                              \
			}                                                                                                                                                                      \
		}                                                                                                                                                                          \
	}                                                                                                                                                                              \
	/**/

	namespace details
	{
		template<typename T, typename T_FACTORY>
		class singleton_base
		{
		public:
			using factory = typename T_FACTORY;
			using type	  = typename T;

			T* operator->()
			{
				return s_instance;
			}

			const T* operator->() const
			{
				return s_instance;
			}

			T& operator*()
			{
				return *s_instance;
			}

			const T& operator*() const
			{
				return *s_instance;
			}

			T* get()
			{
				return s_instance;
			}

			const T* get() const
			{
				return s_instance;
			}

			singleton_base()
			{
				static bool static_init = []() -> bool {
					s_instance = factory::create();
					singleton_cleanup_root()->push([]() -> void {
						delete s_instance;
						s_instance = nullptr;
					});
					return true;
				}();
			}

		private:
			static inline T* s_instance;
		};
	} // namespace details

	template<typename T, typename... T_DEPENDENCIES>
	using singleton = details::singleton_base<T, details::singleton_factory<T, T_DEPENDENCIES...>>;

	template<typename T, typename... T_DEPENDENCIES>
	using virtual_singleton = details::singleton_base<T, details::virtual_singleton_factory<T>>;

	template<typename T_SINGLETON>
	class exported_singleton
	{
	public:
		using singleton_type = typename T_SINGLETON;
		using type			 = typename singleton_type::type;

		type* operator->()
		{
			return s_instance;
		}

		const type* operator->() const
		{
			return s_instance;
		}

		type& operator*()
		{
			return *s_instance;
		}

		const type& operator*() const
		{
			return *s_instance;
		}

		type* get()
		{
			return s_instance;
		}

		const type* get() const
		{
			return s_instance;
		}

		exported_singleton()
		{
			static bool static_init = []() -> bool {
				s_instance = get_instance();
				return true;
			}();
		}

	private:
		static inline type* s_instance;
		static type*		get_instance();
	};

#define imgui_app_EXPORT_SINGLETON(T)                                                                                                                                              \
	template<>                                                                                                                                                                     \
	T::type* T::get_instance()                                                                                                                                                     \
	{                                                                                                                                                                              \
		return singleton_type().get();                                                                                                                                             \
	}                                                                                                                                                                              \
	/**/

#define imgui_app_EXPORT_SINGLETON_DEPS(T, ...)                                                                                                                                    \
	template<>                                                                                                                                                                     \
	T::type* T::get_instance()                                                                                                                                                     \
	{                                                                                                                                                                              \
		::imgui_app::details::singleton_dependencies<__VA_ARGS__>::update();                                                                                                       \
		return singleton_type().get();                                                                                                                                             \
	}                                                                                                                                                                              \
	/**/

	namespace details
	{
		template<typename T>
		class static_root_thread_local_singleton
		{
		public:
			T* operator->()
			{
				return s_instance;
			}
			const T* operator->() const
			{
				return s_instance;
			}
			T& operator*()
			{
				return *s_instance;
			}
			const T& operator*() const
			{
				return *s_instance;
			}

			static_root_thread_local_singleton()
			{
				static bool static_init = []() -> bool {
					s_instance = new (&s_instance_memory[0]) T();
					std::atexit(destroy);
					return true;
				}();
			}

		protected:
			static void destroy()
			{
				if (s_instance)
				{
					s_instance->~T();
					s_instance = nullptr;
				}
			}

		private:
			static inline thread_local uint64_t s_instance_memory[1 + (sizeof(T) / sizeof(uint64_t))];
			static inline thread_local T*		s_instance = nullptr;
		};

		using thread_local_singleton_cleanup_root = static_root_thread_local_singleton<singleton_cleanup_list<1024>>;

		template<typename T, typename T_FACTORY>
		class thread_local_singleton_base
		{
		public:
			using type = typename T;
			T* operator->()
			{
				return s_instance;
			}

			const T* operator->() const
			{
				return s_instance;
			}

			T& operator*()
			{
				return *s_instance;
			}

			const T& operator*() const
			{
				return *s_instance;
			}

			T* get()
			{
				return s_instance;
			}

			const T* get() const
			{
				return s_instance;
			}

			thread_local_singleton_base()
			{
				static thread_local bool static_init = []() -> bool {
					s_instance = new T();
					thread_local_singleton_cleanup_root()->push([]() -> void {
						delete s_instance;
						s_instance = nullptr;
					});
					return true;
				}();
			}

			static inline thread_local T* s_instance;
		};
	} // namespace details

	template<typename T, typename... T_DEPENDENCIES>
	using thread_local_singleton = details::thread_local_singleton_base<T, details::singleton_factory<T, T_DEPENDENCIES...>>;

	template<typename T, typename... T_DEPENDENCIES>
	using thread_local_virtual_singleton = details::thread_local_singleton_base<T, details::virtual_singleton_factory<T>>;

	template<typename T_SINGLETON>
	class exported_thread_local_singleton
	{
	public:
		using singleton_type = typename T_SINGLETON;
		using type			 = typename singleton_type::type;

		type* operator->()
		{
			return s_instance;
		}

		const type* operator->() const
		{
			return s_instance;
		}

		type& operator*()
		{
			return *s_instance;
		}

		const type& operator*() const
		{
			return *s_instance;
		}

		type* get()
		{
			return s_instance;
		}

		const type* get() const
		{
			return s_instance;
		}

		exported_thread_local_singleton()
		{
			static thread_local bool static_init = []() -> bool {
				s_instance = get_instance();
				return true;
			}();
		}

	private:
		static inline thread_local type* s_instance;
		static type*					 get_instance();
	};

#define imgui_app_EXPORT_THREAD_LOCAL_SINGLETON(T)                                                                                                                                 \
	template<>                                                                                                                                                                     \
	::imgui_app::exported_thread_local_singleton<T>::type* ::imgui_app::exported_thread_local_singleton<T>::get_instance()                                                         \
	{                                                                                                                                                                              \
		return singleton_type().get();                                                                                                                                             \
	}                                                                                                                                                                              \
	/**/
} // namespace imgui_app

#include <imgui_app_fw.h>

namespace imgui_app
{
	bool select_platform(imgui_app_fw::platform p);
	void set_window_title(const char* title);
	bool init();
	bool pump();
	void begin_frame();
	void end_frame(ImVec4 clear_color);
	void destroy();

	void log(const char* text) noexcept;
	void warning(const char* text) noexcept;
	void error(const char* text) noexcept;
	void info(const char* text) noexcept;
} // namespace imgui_app

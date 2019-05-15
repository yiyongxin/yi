#include <experimental/coroutine>
#include "../common/log/log_ifs.h"

class wait_some_times 
{
public:
    int left_times;
    std::experimental::coroutine_handle<> handle;
    wait_some_times(int t) : left_times(t), handle(nullptr) {}
};

class suspend_some_times 
{
public:
    wait_some_times& d;
    suspend_some_times(wait_some_times& _d) : d(_d) {}
    bool await_ready() noexcept {
        LOGIFS_INFO("call await_ready: " << d.left_times);
        return d.left_times <= 0;
    }

    void await_suspend(std::experimental::coroutine_handle <> h) noexcept {
        // 记下来handle以便后面resume用
        d.handle = h;

        LOGIFS_INFO("call await_suspend: " << (--d.left_times));
    }

    void await_resume() noexcept {
        LOGIFS_INFO("call await_resume: " << d.left_times);
        d.handle = nullptr;
    }
};

class coroutine_task 
{
public:
    class promise_type 
    {
    public:
        coroutine_task get_return_object() {
            return coroutine_task{};
        }
        auto initial_suspend() {
            return std::experimental::suspend_never{};
        }
        auto final_suspend() {
            return std::experimental::suspend_never{};
        }

        void unhandled_exception() {}
        void return_void() {}
    };
};

auto operator co_await(wait_some_times& x) noexcept {
    return suspend_some_times(x);
}

coroutine_task f(wait_some_times& waiter) {
    LOGIFS_INFO("begin to co_await");
    co_await waiter; // 只有前三次会协程切出
    co_await waiter;
    co_await waiter;
    co_await waiter; // 这之后await_ready返回true了，不会再切出
    co_await waiter;
    LOGIFS_INFO("end of corotine");
}

void co_test()
{
#ifdef __cpp_coroutines
    LOGIFS_INFO("__cpp_coroutines: " << __cpp_coroutines);
#endif
    wait_some_times waiter(3);
    f(waiter);

    while (waiter.handle && !waiter.handle.done()) {
        LOGIFS_INFO("about to resume: " << waiter.left_times);
        // 这里用传出的handle来恢复切入协程
        waiter.handle.resume();
    }
}

// auto fs_open(uv_loop_t* loop, uv_fs_t* req, const char* path, int flags, int mode)
// {
//     promise_t<uv_file> awaitable;
//     auto state = awaitable._state->lock();
//     req->data = state;

//     auto ret = uv_fs_open(loop, req, path, flags, mode,
//     [](uv_fs_t* req) -> void
//     {
//     auto state = static_cast<promise_t<uv_file>::state_type*>(req->data);
//     state->set_value(req->result);
//     state->unlock();
//     });

//     if (ret != 0)
//     {
//     state->set_value(ret);
//     state->unlock();
//     }
//     return awaitable.get_future();
// }
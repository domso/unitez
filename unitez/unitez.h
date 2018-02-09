#ifndef unitez_h
#define unitez_h

#include <vector>
#include <string>
#include <functional>
#include <atomic>
#include <mutex>
#include <condition_variable>

class unitez {
public:
    
    unitez();
    unitez(const unitez& copy) = delete;
    unitez(const unitez&& move) = delete;
    
    template <class T>
    void register_unit_test() {
        test_t newTest;
        
        newTest.name = T::name;
        newTest.module = T::module;
        newTest.check = T::check;
        newTest.flags = T::flags;
        newTest.result = test_result::ready;
        
        m_tests.push_back(newTest);
    }    
    
    struct flags {
        constexpr static const int no_options = 0;
        constexpr static const int singlethread = 1;
    };
    
    void execute(const std::string& selectedModule = "");    
private:    
    void thread_main(const int testID);
    void print_result();
    
    static void signal_handler(int signal);
        
    class semaphore {
    public:
        void set(const int c);
        void wait(const int c = 1);
        void post();
        
        constexpr static const double timeout = 5.0;
    private:
        std::mutex m_mutex;
        std::condition_variable m_cond;
        int m_counter;
    };
    
    enum test_result {
        success,
        error,
        wait,
        ready
    };
    
    struct test_t {
        std::string name;
        std::string module;
        test_result result;
        int flags;
        std::function<bool(void)> check;
    };
    
    semaphore m_semaphore;
    static semaphore* semaphoreRef;
    
    std::vector<test_t> m_tests;
    std::atomic<int> m_testResultCounter;
    static std::atomic<int>* testResultCounterRef;
};

#endif

#include "unitez.h"
#include <iostream>
#include <thread>
#include <signal.h>


unitez::unitez() : m_testResultCounter(0) {
    
}

void unitez::execute(const std::string& selectedModule) {
    testResultCounterRef = &m_testResultCounter;
    semaphoreRef = &m_semaphore;
    
    int numThreads = std::thread::hardware_concurrency();
    m_semaphore.set(numThreads);
    
    for (int i = 0; i < m_tests.size(); i++) {
        if (m_tests[i].module == selectedModule or selectedModule == "") {
            int waitCount = 1;                
            
            if ((m_tests[i].flags & flags::singlethread) != 0) {
                waitCount = numThreads;
            }
            
            m_semaphore.wait(waitCount);        
            std::thread newThread(&unitez::thread_main, this, i);
            newThread.detach();       
            
            if (waitCount > 1) {
                m_semaphore.wait(1);
                m_semaphore.set(numThreads);
            }
        }
    }
    
    m_semaphore.wait(numThreads);
    
    print_result();
}

void unitez::thread_main(const int testID) {    
    struct sigaction t1;    
    t1.sa_handler = &unitez::signal_handler;    
    sigaction(SIGSEGV, &t1, nullptr);
    
    m_tests[testID].result = test_result::wait;
    
    if (m_tests[testID].check()) {
        m_tests[testID].result = test_result::success;
    } else {
        m_tests[testID].result = test_result::error;
    }
    
    m_testResultCounter++;
    m_semaphore.post();
}

void unitez::print_result() {
    int failed = 0;
    int success = 0;
    for (test_t& currentTest : m_tests) {
        if (currentTest.result == test_result::error) {  
            std::cerr << "[Error] Failed unit test '" << currentTest.module << "::" << currentTest.name << "'" << std::endl;
            failed++;
        } else if (currentTest.result == test_result::wait) {   
            std::cerr << "[Error] No response from unit test '" << currentTest.module << "::" << currentTest.name << "'" << std::endl;
            failed++;             
        } else if (currentTest.result == test_result::success) {   
            success++;
        }
    }
    
    std::cout << "Total: " << success << " ok | " << failed << " failed" << std::endl;
}

void unitez::signal_handler(int signal) {
    testResultCounterRef->fetch_add(1);
    semaphoreRef->post();
    // Ugly, but it should do the job   
    pthread_exit(nullptr);
}

void unitez::semaphore::set(const int c) {
    std::unique_lock<std::mutex> ul(m_mutex);
    m_counter = c;
}

void unitez::semaphore::wait(const int c) {
    std::unique_lock<std::mutex> ul(m_mutex);
    while (m_counter < c) {        
        std::chrono::duration<double> duration(timeout);
        if (m_cond.wait_for(ul, duration) != std::cv_status::no_timeout) {
            m_counter += c;
        }
    }
    
    m_counter -= c;
}

void unitez::semaphore::post() {
    std::unique_lock<std::mutex> ul(m_mutex);
    m_counter++;
    m_cond.notify_all();
}

std::atomic<int>* unitez::testResultCounterRef;
unitez::semaphore* unitez::semaphoreRef;

void unit_test_list(unitez& tester);

int main(int argc, char **argv) {
    unitez tester;
    
    unit_test_list(tester);
    
    if (argc == 1) {
        tester.execute();            
    } else {
        tester.execute(argv[1]);
    }    
    
    return 0;
}

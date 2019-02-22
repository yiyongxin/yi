#include <boost/pool/pool.hpp>
#include <boost/pool/singleton_pool.hpp>
#include <sys/time.h>
#include <thread>
#include <memory.h>
#include <jemalloc/jemalloc.h>
#include <stdlib.h>     

#define MALLOC malloc

namespace malloc_test
{

    int max = 1000000;
    int size = 50;
    boost::pool<> tpool(size);
    timeval NowTime,endtime;
    void** buf = new void*[max];
    void** buf_th = new void*[max];

    void single_thread_test()
    {
        for(int d=0;d<4;d++)
        {
    	    gettimeofday(&NowTime, NULL);
            for(int i = 0;i < max;i++)
            {
                buf[i] = tpool.MALLOC();
            // }
            // for(int i = 0;i < max;i++)
            // {
                tpool.free(buf[i]);
            }
            gettimeofday(&endtime, NULL);
            printf("pool: %lld\n",(endtime.tv_sec - NowTime.tv_sec) * 1000 * 1000 + (endtime.tv_usec - NowTime.tv_usec)); 
            // printf("pool: %d:%d  %d:%d\n",endtime.tv_sec,endtime.tv_usec, NowTime.tv_sec,NowTime.tv_usec); 

            gettimeofday(&NowTime, NULL);
            for(int i = 0;i < max;i++)
            {
                buf[i] = MALLOC(size);
            // }
            // for(int i = 0;i < max;i++)
            // {
                free(buf[i]);
            }
            gettimeofday(&endtime, NULL);
            printf("malloc: %lld\n",(endtime.tv_sec - NowTime.tv_sec) * 1000 * 1000 + (endtime.tv_usec - NowTime.tv_usec)); 
            // printf("pool: %d:%d  %d:%d\n",endtime.tv_sec,endtime.tv_usec, NowTime.tv_sec,NowTime.tv_usec); 

            gettimeofday(&NowTime, NULL);
            for(int i = 0;i < max;i++)
            {
                buf[i] = new char[size];
            // }
            // for(int i = 0;i < max;i++)
            // {
                delete buf[i];
            }
            gettimeofday(&endtime, NULL);
            printf("new: %lld\n",(endtime.tv_sec - NowTime.tv_sec) * 1000 * 1000 + (endtime.tv_usec - NowTime.tv_usec)); 
            // printf("pool: %d:%d  %d:%d\n",endtime.tv_sec,endtime.tv_usec, NowTime.tv_sec,NowTime.tv_usec); 

            gettimeofday(&NowTime, NULL);
            for(int i = 0;i < max;i++)
            {
                memcpy(buf[i],buf[0],size); 
            }
            gettimeofday(&endtime, NULL);
            printf("copy: %lld\n",(endtime.tv_sec - NowTime.tv_sec) * 1000 * 1000 + (endtime.tv_usec - NowTime.tv_usec)); 
            // printf("pool: %d:%d  %d:%d\n",endtime.tv_sec,endtime.tv_usec, NowTime.tv_sec,NowTime.tv_usec); 

            printf("==================================\n");         
        }
    }

    int main_int,thread_int;
    int main_int_th;
    void foo()
    {
    	thread_int=0;
        while(thread_int<max)
    	{
    		if (buf[thread_int] != NULL)
    		{
    			// delete buf[thread_int];
                // free(buf[thread_int]);
                // tpool.free(buf[thread_int]);
                buf_th[thread_int]=buf[thread_int];
                thread_int++;
    		}
    	}
    }

    void multi_thread_pool_test()
    {
        gettimeofday(&NowTime, NULL);
        for(main_int=0;main_int<max;main_int++)
	    {
            buf[main_int] = tpool.MALLOC();            
            // buf[main_int] = malloc(size);
	    	// buf[main_int] = new char[size];
	    // }
        // for(main_int=0;main_int<max;main_int++)
	    // {
            tpool.free(buf[main_int]);            
            // free(buf[main_int]);
            // delete buf[main_int];
	    }
        gettimeofday(&endtime, NULL);
        printf("multi_1: %lld\n",(endtime.tv_sec - NowTime.tv_sec) * 1000 * 1000 + (endtime.tv_usec - NowTime.tv_usec)); 
        // printf("multi_1: %d:%d  %d:%d\n",endtime.tv_sec,endtime.tv_usec, NowTime.tv_sec,NowTime.tv_usec); 
        memset(buf, NULL, max);
        memset(buf_th, NULL, max);
        std::thread first(foo);
        int conut = 0;        
        gettimeofday(&NowTime, NULL);
	    main_int=0;
        main_int_th=0;
        while(main_int_th<max)
	    {
            if(buf_th[main_int_th] != NULL)
            {
                tpool.free(buf_th[main_int_th]);
                main_int_th++;
            }
            else if(main_int<max)
            {
	    	    // buf[main_int] = new char[size];
                // buf[main_int] = MALLOC(size);
                buf[main_int] = tpool.MALLOC();
                main_int++;
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                conut++;
            }
	    }
        // printf("multi pool: %d:%d  %d:%d\n",endtime.tv_sec,endtime.tv_usec, NowTime.tv_sec,NowTime.tv_usec); 
        first.join();
        gettimeofday(&endtime, NULL);
        printf("multi pool: %lld conut: %d\n",(endtime.tv_sec - NowTime.tv_sec) * 1000 * 1000 + (endtime.tv_usec - NowTime.tv_usec),conut); 
        printf("==================================\n");    
    }

    void foo_2()
    {
        int conut = 0;
    	thread_int=0;
        while(thread_int<max)
    	{
    		if (buf[thread_int] != NULL)
    		{
    			// delete buf[thread_int];
                free(buf[thread_int]);
                thread_int++;
    		}
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                if(main_int != 0)
                    printf("----nnnnnnnnnn----%d-----%d---------\n",thread_int,main_int);      
                conut++;
            }
    	}
        gettimeofday(&endtime, NULL);
        printf("multi: %lld conut: %d\n",(endtime.tv_sec - NowTime.tv_sec) * 1000 * 1000 + (endtime.tv_usec - NowTime.tv_usec),conut); 
        // printf("multi: %d:%d  %d:%d\n",endtime.tv_sec,endtime.tv_usec, NowTime.tv_sec,NowTime.tv_usec); 
        printf("==================================\n");      
    }

    void multi_thread_test()
    {
        gettimeofday(&NowTime, NULL);
        for(main_int=0;main_int<max;main_int++)
	    {
            // buf[main_int] = tpool.MALLOC();            
            buf[main_int] = MALLOC(size);
	    	// buf[main_int] = new char[size];
	    // }
        // for(main_int=0;main_int<max;main_int++)
	    // {
            // tpool.free(buf[main_int]);            
            free(buf[main_int]);
            // delete buf[main_int];
	    }
        gettimeofday(&endtime, NULL);
        printf("multi_1: %lld\n",(endtime.tv_sec - NowTime.tv_sec) * 1000 * 1000 + (endtime.tv_usec - NowTime.tv_usec)); 
        // printf("multi_1: %d:%d  %d:%d\n",endtime.tv_sec,endtime.tv_usec, NowTime.tv_sec,NowTime.tv_usec); 

        memset(buf, NULL, max);
	    main_int=0;
        std::thread first(foo_2);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        gettimeofday(&NowTime, NULL);
        for(main_int=0;main_int<max;main_int++)
	    {
            // printf("----mmmmmmm----%d----------------\n",main_int);      
	    	// buf[main_int] = new char[size];
            buf[main_int] = MALLOC(size);
	    }

        first.join();
    }

    void test()
    {
        printf("==================================\n");     
        // try
        // { 
            single_thread_test();
            multi_thread_pool_test();
            multi_thread_test();
        // }
        // catch(...)
        // {

        // }
    }
}
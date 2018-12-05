using namespace std;

class ThreadPool{

    public:

    explicit ThreadPool ( int threads) : shut ( false){

        threads_.reserve( threads);

        for( int i = 0; i < threads; ++i)
            threads_.emplace_back( bind( &ThreadPool::entry, this, i));
    }

    ~ThreadPool(){

        {
            unique_lock <mutex> l (lock_);

            shut = true;
            condVar_.notify_all();
        }

        for (auto& thread : threads_) thread.join();
    }

    void work( function <void (void)> func){

        unique_lock <mutex> l (lock_);

        jobs_.emplace( move (func));
        condVar_.notify_one();
    }

    void lock (){

        unique_lock <mutex> l (lock_);
    }

    void unlock (){

        unique_lock <mutex> l (lock_);
    }

    protected:

    void entry( int i){

        function <void (void)> job;

        while( true){

            {
                unique_lock <mutex> l (lock_);

                while (! shut && jobs_.empty())
                    condVar_.wait (l);

                if (jobs_.empty ()){

                    return;
                 }

                job = move (jobs_.front ());

                jobs_.pop();
            }

            job( );
        }

    }

    bool shut;
    queue <function <void (void)>> jobs_;
    vector <thread> threads_;
    mutex lock_;
    condition_variable condVar_;
};


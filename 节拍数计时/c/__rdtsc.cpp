        for (int i = start; i < end -10; i+=32)
        {
                //unsigned long long t1, t2;
               
                volatile unsigned long long t1 = __rdtsc();

                code;

                volatile unsigned long long t2 = __rdtsc();
             
                if(i == 32) cout << t2 - t1<< endl;
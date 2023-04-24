import java.io.*;
class fibonacci {
    static int fib(int n)
    {
        int x;
        if (n <= 1)
            x = 1;
        else {
            x = fib(n-1);
            x = x + fib(n-2);
        }
        return x;
    }
 
    public static void main(String args[])
    {
        int n = 7;
        int x =fib(n);
        System.out.println(x);
    }
}
/* This code is contributed by Rajat Mishra */
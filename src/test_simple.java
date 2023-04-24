//simple test for function-calls, object creation
class hello {
    int c;
    hello(int a, int b)
    {
        c = a;
    }
    int f(int a, int b) {
        int c=2;
        // int c = b/a;
        if(a==1)
        {
            b++;
        }
        return a++;
    }
}

class hi{
    int T(int a, int b) {
        int n = a+b;
        int k = a-b;
        hello h = new hello(n, k);
        h.f(n, k);
        return 1;
    }
    public static void main(String args[]){
        int y=T(1,2);
        System.out.println(y);
    }
}

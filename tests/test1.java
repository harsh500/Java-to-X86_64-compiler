//simple test for function-calls, object creation
class hello {
    int c;
    int f(int a, int b) {
        // int c=2;
        if(a==1)
        {
            b++;
            int c = 1;
        }
        return a++;
    }
}

class hi{
    int T(int a, int b) {
        int n = a+b;
        int k = a-b;
        hello h = new hello();
        int t=h.f(n,k);
        return t;
    }
    void main(){
        int y=T(1,2);
        System.out.println(y);
    }
}

//test for static variables and functions

class test9{
    int count=0;
    test9(int a)
    {
        count = a;
    }
    public void func1(int a)
    {
        a = 2;
        if(a==1)
            a+=3;
    }

    public static int call(int b, int c)
    {
        int sum = b+c;
        int diff = b-c;
        System.out.println(sum);
        int x;
        if(b>0)
            x= sum;
        else 
            x=diff;
        return x;
    }
}

class test{
    void func2(int a)
    {
        //test9.count++;
        
    }
    public static void main(String args[])
    {
        int a =2;
        test9 ob = new test9(a);
        ob.func1(a);
        ob.call(a, a+1);
    }
}

//basic if and while loop
public class test7 
{  
  public static void main(String args[])  
  {  
    int x=1;
    int y=2;
    int n = x+y;
    int k = x-y;
    int c;
    if(n<k){
      c=x;
    }
    else{
      c=y;
    }
    while(c>0){
      System.out.println(c);
      c--;
    }
  }  
}
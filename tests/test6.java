//Test for short circuiting
public class test6 {
    int printnum(){
        System.out.println(3);
        return 1;
    }
    int printchar(){
        System.out.println(4);
        return 2;
    }
    int printdouble(){
        System.out.println(1);
        return 3;
    }
    int main(){
        if(printchar()<3||printnum()>2||printdouble()>1){return 1;}
        else if((printchar()<3||printnum()>2)&&printdouble()>1){
            return 2;
        }
        return 5;
    }
}

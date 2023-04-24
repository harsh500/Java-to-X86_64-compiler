//test for multidimensional array
public class test5 {
    public int sum(int a[]){
        int ans=0;
            for(int j=0;j<3;++j){
                ans+=a[j];
            }
        
        return ans;
    }
    public int main(){
        int arr[]=new int [3];
        int i;
        for(i = 0; i < 3; i++) arr[i]=i;
        //arr[2] = i+2;
        int u=sum(arr);
        System.out.println(u);
        // if(++u>5){
        //     return u;
        // }
        // else if(u++<4){
        //     return u;
        // }
        // else if(u==10){
        //     return u+4;
        // }
        // else{
        //     return arr[0][0];
        // }
        return 0;
    }
}

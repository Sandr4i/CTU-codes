package cz.cvut.fel.pjv;

public class Main {
    public static void main(String[] args) {
        TreeImpl t = new TreeImpl();
        //int[] arr = {1,2,3,4,5,6,7};
        int[] arr2 = {};
        t.setTree(arr2);
        System.out.println(t.toString());
    }
}

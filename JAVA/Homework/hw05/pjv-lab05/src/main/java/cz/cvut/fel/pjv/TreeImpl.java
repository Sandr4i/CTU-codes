package cz.cvut.fel.pjv;

public class TreeImpl implements Tree{
    private Node root;
    public TreeImpl(){
        this.root = new NodeImpl();
    }

    @Override
    public void setTree(int[] values){
        root = makeTree(values, 0, values.length);
    }

    private Node makeTree(int[] values, int head, int tail){
        if (head > tail || tail == head){
            return null;
        }
        int midIdx = (head + tail)/2;  // index of middle number in array
        NodeImpl n = new NodeImpl();
        n.setValue(values[midIdx]);
        n.setLeft(makeTree(values, head, midIdx));
        n.setRight(makeTree(values, midIdx + 1, tail));
        return n;
    }

    @Override
    public Node getRoot() {
        return root;
    }

    @Override
    public String toString(){
        return returnTree(root, 0);
    }
    private String returnTree(Node n, int layer){
        if (n == null){
            return "";
        }
        String ret = "";
        for (int i = 0; i < layer; i++){
            ret+=" ";
        }
        ret+="- " + n.getValue() + "\n";
        if (n.getLeft()!=null){
            ret+=returnTree(n.getLeft(), layer+1);
        }
        if (n.getRight()!=null){
            ret+=returnTree(n.getRight(), layer+1);
        }
        return ret;
    }
}

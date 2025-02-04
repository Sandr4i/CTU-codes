package cz.cvut.fel.pjv;

public class NodeImpl implements Node{
    private int value;
    private Node right;
    private Node left;

    @Override
    public Node getLeft(){
        return left;
    }

    @Override
    public Node getRight(){
        return right;
    }

    @Override
    public int getValue(){
        return value;
    }

    public void setValue(int value){   //sets value into the node
        this.value = value;
    }

    public void setRight(Node right){  // sets link to the child right node
        this.right = right;
    }

    public void setLeft(Node left){ // sets link to the child left node
        this.left = left;
    }
}

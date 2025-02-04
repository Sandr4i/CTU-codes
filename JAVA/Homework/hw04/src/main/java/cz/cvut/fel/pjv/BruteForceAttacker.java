package cz.cvut.fel.pjv;

public class BruteForceAttacker extends Thief {
    private int size;
    private char[] chars;
    private char[] password;
    private boolean passwordGuessed = false;

    @Override
    public void breakPassword(int sizeOfPassword) {
        // write your code
        this.size = sizeOfPassword;
        char[] set = new char[sizeOfPassword];
        char[] chars = getCharacters();
        guessPassword(set, chars, 0);
    }

    private void guessPassword(char[] set, char[] chars, int position) {
        if (isOpened()) return;
        if (size == position) {
            if (tryOpen(set)) {
                passwordGuessed = true;
                password = set;
                return;
            } else { return; }
        }
        for (int i = 0; i < chars.length; i++){
            set[position] = chars[i];
            guessPassword(set, chars, position + 1);
        }
    }
}

package lab1;
import java.math.BigInteger;
import java.util.Scanner;

public class BinaryPower {

    private static final BigInteger ZERO = BigInteger.valueOf(0);
    private static final BigInteger ONE = BigInteger.valueOf(1);
    private static final BigInteger TWO = BigInteger.valueOf(2);

    public static void main(String[] args) {

        Scanner scan = new Scanner(System.in);
        BigInteger numb = scan.nextBigInteger();
        BigInteger deg = scan.nextBigInteger();
        BigInteger mod = scan.nextBigInteger();

        System.out.println(bInPow(numb, deg).mod(mod));
    }

    public static BigInteger bInPow(BigInteger a, BigInteger n) {
        if (n.equals(ZERO)) {
            return BigInteger.ONE;
        }
        if (n.mod(TWO).equals(ONE)) {
            return a.multiply(bInPow(a, n.subtract(ONE)));
        } else {
            BigInteger b = bInPow(a, n.divide(TWO));
            return b.multiply(b);
        }
    }
}
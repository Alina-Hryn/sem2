package lab1;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.math.BigInteger;
import java.util.Random;


public class Montgomery {

    public static void main(String[] args) throws IOException {
        // Prompt user on standard output, parse standard input
        Random random = new Random(100);
        int operRand = random.nextInt() % 2;
        BigInteger x = new BigInteger(200, random);
        String oper = operRand == 0? "mult":"pow";
        BigInteger y = new BigInteger(200, random);
        BigInteger mod = new BigInteger(200, random);
        BigInteger z = computationMontgomery(x, y, oper, mod);
        System.out.printf("%d%s%d mod %d%n", x, oper.equals("times") ? " * " : "^", y, mod);
        System.out.println("= " + z);
    }

    public static BigInteger computationMontgomery(BigInteger x, BigInteger y, String operation, BigInteger mod) {
        MontgomeryReducer red = new MontgomeryReducer(mod);
        BigInteger xm = red.convertIn(x);
        BigInteger zm;
        BigInteger z;
        if (operation.equals("mult")) {
            zm = red.multiply(xm, red.convertIn(y));
            z = x.multiply(y).mod(mod);
        } else if (operation.equals("pow")) {
            zm = red.pow(xm, y);
            z = x.modPow(y, mod);
        } else
            throw new IllegalArgumentException("Invalid operation: " + operation);
        if (!red.convertOut(zm).equals(z))
            throw new AssertionError("Self-check failed");
        return z;

    }

    public static BigInteger powBigInteger(BigInteger base, BigInteger exponent) {
        BigInteger result = BigInteger.ONE;
        while (exponent.signum() > 0) {
            if (exponent.testBit(0)) result = result.multiply(base);
            base = base.multiply(base);
            exponent = exponent.shiftRight(1);
        }
        return result;
    }
}


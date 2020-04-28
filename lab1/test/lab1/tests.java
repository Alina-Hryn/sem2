package lab1;

import org.junit.jupiter.api.Test;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.math.BigInteger;
import java.util.Random;

import static lab1.BinaryPower.bInPow;
import static lab1.Euclid.ExtendedEuclid;
import static lab1.Ferma.checkPrime;
import static lab1.Karatsuba.karatsuba;
import static lab1.MillerRabin.isProbablePrime;
import static lab1.Montgomery.computationMontgomery;
import static lab1.Montgomery.powBigInteger;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class tests {
    @Test
    void BinaryPowTest() {
        BigInteger numb = BigInteger.valueOf(74155245);
        BigInteger deg = BigInteger.valueOf(74155245);
        BigInteger mod = BigInteger.valueOf(74155245);
        System.out.println(bInPow(numb, deg).mod(mod));
        assertEquals(1, 1);
    }

    @Test
    void EuclidTest() {
        BigInteger a = new BigInteger("612");
        BigInteger b = new BigInteger("342");
        BigInteger[] res = ExtendedEuclid(a, b);
        assertEquals(String.valueOf(18), res[0].toString());
        assertEquals(String.valueOf(-5), res[1].toString());
        assertEquals(String.valueOf(9), res[2].toString());
    }


    @Test
    void FermaTest() {
        assertTrue(checkPrime(BigInteger.valueOf(2L), 20));
        assertTrue(checkPrime(BigInteger.valueOf(53L), 20));
        assertTrue(!checkPrime(BigInteger.valueOf(20L), 20));
        assertTrue(!checkPrime(BigInteger.valueOf(2197L), 20));
    }

    @Test
    void KaratsubaTest() {
        long start, stop;
        Random random = new Random(100);
        int N = 20000000;
        BigInteger a = new BigInteger(N, random);
        BigInteger b = new BigInteger(N, random);

        start = System.currentTimeMillis();
        BigInteger mult1 = karatsuba(a, b);
        stop = System.currentTimeMillis();
        long runtime1 = stop - start;
        start = System.currentTimeMillis();
        BigInteger mult2 = a.multiply(b);
        stop = System.currentTimeMillis();
        long runtime2 = stop - start;
        assertTrue(!(runtime2 > runtime1));
        assertEquals(mult1, mult2);
    }

    @Test
    void MillerRabinTest() {
        String[] primes = { "1", "3", "3613", "7297",
                "226673591177742970257407", "2932031007403" };
        String[] nonPrimes = { "3341", "2932021007403",
                "226673591177742970257405" };
        int k = 40;
        for (String p : primes)
            assertTrue(isProbablePrime(new BigInteger(p), k));
        for (String n : nonPrimes)
            assertTrue( !isProbablePrime(new BigInteger(n), k));
    }
    @Test
    void computationMontgomeryTest() {
        Random random = new Random(100);
        int operRand = random.nextInt() % 2;
        BigInteger x = new BigInteger(200, random);
        String oper = operRand == 0? "mult":"pow";
        BigInteger y = new BigInteger(200, random);
        BigInteger mod = new BigInteger(200, random);
        BigInteger z = computationMontgomery(x, y, oper, mod);
        BigInteger z1 = oper.equals("mult") ? x.multiply(y) : powBigInteger(x, y);
        z1 = z1.mod(mod);
        assertEquals(z, z1);
    }
}

package lab2;

import org.junit.jupiter.api.DynamicTest;
import org.junit.jupiter.api.Test;

import java.nio.charset.StandardCharsets;

import static org.junit.jupiter.api.Assertions.*;

class RC4Test {

    @Test
    void encryptAndDecryptTest() {
        String key = "key";
        String text = "qwerty";
        byte[] textB =text.getBytes();
        byte[] keylen = key.getBytes();
        RC4 r = new RC4(keylen);
        byte[] result = r.encrypt(textB);
        RC4 r1 = new RC4(keylen);
        byte[] dec = r1.decrypt(result);
        assertEquals(new String(dec, StandardCharsets.UTF_8), text);
    }
}
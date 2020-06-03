package univ;


import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;
import static org.junit.jupiter.api.Assertions.assertEquals;

class RSATest {
    @Test
    public void encryptAndDecryptTest() {
        RSA tool = new RSA(256, "4567");
        String value = "24";
        String result = tool.encrypt(value);
        result = tool.decrypt(result);
        assertEquals(result, value);
    }

    @Test
    public void encryptDifferentTest() {
        RSA tool = new RSA(256, "4567");
        RSA tool1 = new RSA(256, "4567");
        String value = "24";
        String result = tool.encrypt(value);
        String resultForTool1 = tool1.encrypt(value);
        assertNotEquals(result, resultForTool1);
    }

    @Test
    public void decryptDifferentTest() {
        RSA tool = new RSA(256, "4567");
        RSA tool1 = new RSA(256, "4567");
        String value = "24";
        String result = tool.decrypt(value);
        String resultForTool1 = tool1.decrypt(value);
        assertNotEquals(result, resultForTool1);
    }


}
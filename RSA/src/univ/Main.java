package univ;

import java.util.Scanner;

public class Main {

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        RSA tool = new RSA(Integer.parseInt(scanner.nextLine()), scanner.nextLine());
        if (scanner.hasNextLine()) {
            String result = scanner.nextLine();
            String result1 = tool.encrypt(result);
            System.out.println(result1);
            result1 = tool.decrypt(result);
            System.out.println(result1);
        }
    }
}

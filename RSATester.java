import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Reader;
import java.math.BigInteger;

public class RSATester {
    
    protected static int sLine;
    protected static BufferedReader reader;
    
    public static void main(String args[]) throws Exception {
        reader = new TestBufferedReader(new InputStreamReader(new FileInputStream(new File("test_out.txt"))));
        String line;
        while ((line = reader.readLine()) != null) {
            if (line.equals("test mul")) {
                testMul();
            } else if (line.equals("test shiftSub")) {
                testShiftSub();
            } else if (line.equals("test mode")) {
                testMode();
            } else if (line.equals("test modPow")) {
                testModPow();
            } else {
                throw new Exception("line not regnised. lino: " + sLine);
            }
        }
    }
    
    private static void testMode() throws Exception {
        int lino = sLine;
        String strA =  reader.readLine();
        String strN =  reader.readLine();
        String strO =  reader.readLine();
        
        BigInteger a = new BigInteger(strA, 16);
        BigInteger N = new BigInteger(strN, 16);
        
        BigInteger res = a.mod(N);
        String strRes = res.toString(16);
        
        if (!strRes.equals(strO)) {
            p("testMode lino: " + lino);
            p("a: " + strA);
            p("N: " + strN);
            p("res: " + strRes);
            p("o: " + strO);
        } 
    }

    private static void testModPow() throws Exception {
        int lino = sLine;
        String strA =  reader.readLine();
        String strB =  reader.readLine();
        String strN =  reader.readLine();
        String strO =  reader.readLine();
        
        BigInteger a = new BigInteger(strA, 16);
        BigInteger b = new BigInteger(strB, 16);
        BigInteger N = new BigInteger(strN, 16);
        
        BigInteger res = a.modPow(b, N);
        String strRes = res.toString(16);
        
        if (!strRes.equals(strO)) {
            p("testModPow lino: " + lino);
            p("a: " + strA);
            p("b: " + strB);
            p("N: " + strN);
            p("res: " + strRes);
            p("o: " + strO);
        } 
    }

    private static void testShiftSub() throws Exception {
        int lino = sLine;
        String strShit =  reader.readLine();
        String strA =  reader.readLine();
        String strB =  reader.readLine();
        String strO =  reader.readLine();
        
        int shift = Integer.valueOf(strShit);
        BigInteger a = new BigInteger(strA, 16);
        BigInteger b = new BigInteger(strB, 16);
        
        b = b.shiftLeft(shift);
        BigInteger res = a.subtract(b);
        String strRes = res.toString(16);
        
        if (!strRes.equals(strO)) {
            p("testShiftSub lino: " + lino);
            p("shift: " + shift);
            p("a: " + strA);
            p("b: " + strB);
            p("res: " + strRes);
            p("o: " + strO);
        } 
    }

    private static void testMul() throws Exception {
        int lino = sLine;
        String strA =  reader.readLine();
        String strB =  reader.readLine();
        String strN =  reader.readLine();
        String strAMulB = reader.readLine();
        
        BigInteger a = new BigInteger(strA, 16);
        BigInteger b = new BigInteger(strB, 16);
        BigInteger N = new BigInteger(strN, 16);
        
        BigInteger aMulB = a.multiply(b);
        String strRes = aMulB.toString(16);
        
        if (!strRes.equals(strAMulB)) {
            p("testMul lino: " + lino);
            p("a: " + strA);
            p("b: " + strB);
            p("N: " + strN);
            //p("aMulB: " + aMulB.toString(16));
            p("res: " + strRes);
            p("aMulB: " + strAMulB);
            //p("o: " + strO);
        } 
    }

    public static void p(String s) {
        System.out.println(s);
    }
    
    private static class TestBufferedReader extends BufferedReader {

        public TestBufferedReader(Reader in) {
            super(in);
        }
        @Override
        public String readLine() throws IOException {
            String l = super.readLine();
            sLine++;
            while (l!=null && l.startsWith("#")) {
                l = super.readLine();
                sLine++;
            }
            return l;
        }
    }
}


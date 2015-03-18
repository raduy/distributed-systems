package pl.agh.edu.sr.raduj.solutions.first;

import java.io.*;
import java.math.BigInteger;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Arrays;

import static java.lang.String.format;

/**
 * @author Lukasz Raduj <raduj.lukasz@gmail.com>
 */
public class PiServer {

    private static final int DEFAULT_PORT = 4444;

    public static void main(String[] args) throws IOException {

        int portNumber;
        if (args.length == 1) {
            portNumber = readPortNumberFromCmd(args);
        } else {
            portNumber = DEFAULT_PORT;
        }

        startListening(portNumber);
    }

    private static void startListening(int portNumber) {
        try (ServerSocket serverSocket = new ServerSocket(portNumber)) {

            println("Server listening on port # %d", portNumber);
            while (!Thread.interrupted()) {
                listenForClient(serverSocket);

            }
        } catch (IOException e) {
            println("Exception caught when trying to listen on port "
                    + portNumber + " or listening for a connection");
            println(e.getMessage());
        }
    }

    private static int readPortNumberFromCmd(String[] args) {
        try {
            return Integer.parseInt(args[0]);
        } catch (NumberFormatException e) {
            println("Invalid port number! Starting on default port...");
            return DEFAULT_PORT;
        }
    }

    private static void listenForClient(ServerSocket serverSocket) {
        try (Socket clientSocket = serverSocket.accept();
             OutputStream outputStream = clientSocket.getOutputStream();
             BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()))) {

            println("Client connected on socket! %s", clientSocket);
            String inputLine;

            while ((inputLine = in.readLine()) != null) {
                println("Requested received");

                System.out.println(Arrays.toString(inputLine.getBytes()));

                long n = parseRequestedDigit(inputLine.getBytes());
                byte nthDigitOfPi = computeNthDigitOfPi(n);

                println("Client requested %d th digit of PI which is: %d", n, nthDigitOfPi);

                outputStream.write(new byte[]{nthDigitOfPi, '\n', '\r'});
            }

        } catch (Exception e) {
            println("Exception on connection with client. Cause: %s", e);
        }

        println("Action successful. Connection closed");
        println("Waiting for another request...");
    }

    private static long parseRequestedDigit(byte[] bytes) {
        int digitSize = bytes[0];
        byte[] digitSizePart = Arrays.copyOfRange(bytes, 1, digitSize + 1);
        return byteArrayToInt(digitSizePart);
    }

    public static long byteArrayToInt(byte[] b) {
        return new BigInteger(b).longValue();
    }

    private static byte computeNthDigitOfPi(long n) {
        byte[] pi = {3, 1, 4, 1, 5};

        if (n <= 0 || n >= pi.length) {
            return 5; // black magic
        }

        return pi[(int) n];
    }

    private static void println(String toPrint, Object... args) {
//        Could be replaced with logger
        System.out.println(String.format(toPrint, args));
    }
}
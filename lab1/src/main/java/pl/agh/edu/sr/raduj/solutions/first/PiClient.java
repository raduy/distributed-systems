package pl.agh.edu.sr.raduj.solutions.first;

import java.io.*;
import java.math.BigInteger;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Arrays;

public class PiClient {
    private static final String DEFAULT_HOSTNAME = "127.0.0.1";
    private static final int DEFAULT_PORT = 4444;

    public static void main(String[] args) throws IOException {

        String hostName = DEFAULT_HOSTNAME;
        int portNumber = DEFAULT_PORT;

        if (args.length == 2) {
            hostName = args[0];
            portNumber = Integer.parseInt(args[1]);
        }

        try (
                Socket echoSocket = new Socket(hostName, portNumber);
                OutputStream outputStream = echoSocket.getOutputStream();

                PrintWriter out =
                        new PrintWriter(outputStream, true);
                BufferedReader in =
                        new BufferedReader(
                                new InputStreamReader(echoSocket.getInputStream()));
                BufferedReader stdIn =
                        new BufferedReader(
                                new InputStreamReader(System.in))
        ) {
            String userInput;
            while ((userInput = stdIn.readLine()) != null) {
                if (userInput.isEmpty()) {
                    continue;
                }

                System.out.println(Arrays.toString(userInput.getBytes()));
                long requestedDigit = Long.parseLong(userInput);

                System.out.println(String.format("Asking for %d th digit of PI", requestedDigit));

                byte[] request = prepareRequest(requestedDigit);
                outputStream.write(request);
                out.println();

                System.out.println("Server claims that it is: " + Arrays.toString(in.readLine().getBytes()));
            }
        } catch (UnknownHostException e) {
            System.err.println("Don't know about host " + hostName);
            System.exit(1);
        } catch (IOException e) {
            System.err.println("Couldn't get I/O for the connection to " +
                    hostName);
            System.exit(1);
        }
    }

    private static byte[] prepareRequest(long i) {
        byte sizePart = prepareSizePart(i);
        byte[] valuePart = new BigInteger(String.valueOf(i)).toByteArray();

        byte[] result = new byte[valuePart.length + 1];

        result[0] = sizePart;
        System.arraycopy(valuePart, 0, result, 1, valuePart.length);

        return result;
    }

    private static byte prepareSizePart(long i) {
        byte sizePart;
        if (i >> 8 == 0) {
            sizePart = (byte) 1;
        } else if (i >> 16 == 0) {
            sizePart = (byte) 2;
        } else if (i >> 32 == 0) {
            sizePart = (byte) 4;
        } else {
            sizePart = (byte) 8;
        }
        return sizePart;
    }

}
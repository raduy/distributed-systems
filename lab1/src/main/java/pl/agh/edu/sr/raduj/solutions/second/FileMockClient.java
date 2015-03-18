package pl.agh.edu.sr.raduj.solutions.second;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Arrays;

public class FileMockClient {
    private static final String DEFAULT_HOSTNAME = "127.0.0.1";
    private static final int DEFAULT_PORT = 4444;

    public static void main(String[] args) throws IOException {

        String hostName = DEFAULT_HOSTNAME;
        int portNumber = DEFAULT_PORT;

        if (args.length == 2) {
            hostName = args[0];
            portNumber = Integer.parseInt(args[1]);
        }

        try (Socket echoSocket = new Socket(hostName, portNumber);
             OutputStream outputStream = echoSocket.getOutputStream();

             BufferedReader in =
                     new BufferedReader(new InputStreamReader(echoSocket.getInputStream()))
        ){


            outputStream.write(new byte[]{'a', '\n'});

            String serverMessage;
            while ((serverMessage = in.readLine()) != null) {
                System.out.println(Arrays.toString(serverMessage.getBytes()));
                System.out.println(serverMessage);
            }

        }catch(UnknownHostException e){
            System.err.println("Don't know about host " + hostName);
            System.exit(1);
        }catch(IOException e){
            System.err.println("Couldn't get I/O for the connection to " +
                    hostName);
            System.exit(1);
        }
    }
}
import java.io.*;
import java.net.*;

public class ClientInt {
    public static void main(String[] args) throws Exception {
        if (args.length != 3) {
            System.out.println("Uso: java ClientInt <ip/host> <puerto> <numeroInicial>");
            return;
        }
        String host = args[0];
        int port = Integer.parseInt(args[1]);

        try (Socket s = new Socket(host, port);
             DataOutputStream out = new DataOutputStream(s.getOutputStream());
             DataInputStream in = new DataInputStream(s.getInputStream());
             BufferedReader teclado = new BufferedReader(new InputStreamReader(System.in))) {

            System.out.println("Conectado al servidor. Envia enteros; 0 termina.");

            while (true) {
                System.out.print("Entero a enviar: ");
                String line = teclado.readLine();
                if (line == null) break;

                int x = Integer.parseInt(line.trim());
                out.writeInt(x);       // Big-endian (network order)
                out.flush();

                if (x == 0) {
                    System.out.println("Terminado por 0.");
                    break;
                }

                int y = in.readInt();
                System.out.println("Servidor respondio: " + y);
            }
        }
    }
}
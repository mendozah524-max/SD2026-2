import java.io.*;
import java.net.*;

public class ServerText {
    public static void main(String[] args) throws Exception {
        if (args.length != 1) {
            System.out.println("Uso: java ServerText <puerto>");
            return;
        }
        int port = Integer.parseInt(args[0]);

        try (ServerSocket server = new ServerSocket(port)) {
            System.out.println("Servidor Java (texto) escuchando en puerto " + port);

            while (true) {
                try (Socket client = server.accept();
                     BufferedReader in = new BufferedReader(new InputStreamReader(client.getInputStream(), "UTF-8"));
                     BufferedWriter out = new BufferedWriter(new OutputStreamWriter(client.getOutputStream(), "UTF-8"))) {

                    System.out.println("Cliente conectado: " + client.getInetAddress());

                    String line;
                    while ((line = in.readLine()) != null) {
                        System.out.println("Recibido: " + line);

                        // Respuesta ejemplo
                        String respuesta = "Hola que tal. Tu dijiste: " + line;
                        out.write(respuesta);
                        out.write("\n");
                        out.flush();

                        if (line.equalsIgnoreCase("exit")) {
                            System.out.println("Cliente pidió salir.");
                            break;
                        }
                    }
                } catch (IOException e) {
                    System.out.println("Error con cliente: " + e.getMessage());
                }
            }
        }
    }
}
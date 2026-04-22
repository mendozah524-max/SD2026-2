import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;

public class ClienteWeb {
    public static void main(String[] args) {
        try {
            HttpClient client = HttpClient.newHttpClient();

            System.out.println("--- PRUEBA DE SERVICIOS WEB ---");

            // 1. Ejecutar operación Pagar con valores de prueba [cite: 27, 28]
            String jsonPagar = "{\"numero_tarjeta\": 123456789, \"monto\": 1000, \"nombre\": \"Juan Perez\", \"codigo_CVV\": 456}";
            HttpRequest requestPagar = HttpRequest.newBuilder()
                    .uri(URI.create("http://localhost:8080/pagar"))
                    .header("Content-Type", "application/json")
                    .POST(HttpRequest.BodyPublishers.ofString(jsonPagar))
                    .build();

            HttpResponse<String> responsePagar = client.send(requestPagar, HttpResponse.BodyHandlers.ofString());
            
            // Validamos el valor de retorno booleano
            if (responsePagar.body().contains("\"exito\":true") || responsePagar.body().contains("\"exito\": true")) {
                System.out.println("TRANSACCION EXITOSA"); // [cite: 32]
            } else {
                System.out.println("FALLÓ LA TRANSACCIÓN"); // [cite: 33]
            }

            // 2. Ejecutar operación Comprar con valores de prueba [cite: 34]
            String jsonComprar = "{\"id_producto\": 101, \"precio\": 250, \"numero_productos\": 4, \"total\": 1000}";
            HttpRequest requestComprar = HttpRequest.newBuilder()
                    .uri(URI.create("http://localhost:8080/comprar"))
                    .header("Content-Type", "application/json")
                    .POST(HttpRequest.BodyPublishers.ofString(jsonComprar))
                    .build();

            HttpResponse<String> responseComprar = client.send(requestComprar, HttpResponse.BodyHandlers.ofString());

            // Validamos el valor de retorno booleano
            if (responseComprar.body().contains("\"exito\":true") || responseComprar.body().contains("\"exito\": true")) {
                System.out.println("COMPRA EXITOSA"); // [cite: 39]
            } else {
                System.out.println("FALLÓ LA COMPRA");
            }

        } catch (Exception e) {
            System.err.println("Error al conectar con los Web Services: " + e.getMessage());
        }
    }
}
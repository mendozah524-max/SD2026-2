from flask import Flask, request, jsonify

app = Flask(__name__)

# Método 1: Pagar [cite: 7]
@app.route('/pagar', methods=['POST'])
def pagar():
    data = request.json
    # Parámetros recibidos [cite: 10, 11, 12, 13, 14]
    numero_tarjeta = data.get('numero_tarjeta')
    monto = data.get('monto', 0)
    nombre = data.get('nombre')
    codigo_cvv = data.get('codigo_CVV')
    
    # Lógica simulada: Si el monto es menor o igual a 5000, la tarjeta tiene fondos suficientes [cite: 29]
    # Retorna true si es exitosa, false en caso contrario [cite: 15, 30]
    transaccion_exitosa = monto <= 5000
    
    print(f"Procesando pago de {nombre} por ${monto}...")
    return jsonify({'exito': transaccion_exitosa})

# Método 2: Comprar [cite: 16]
@app.route('/comprar', methods=['POST'])
def comprar():
    data = request.json
    # Parámetros recibidos [cite: 18, 20, 21, 22, 23]
    id_producto = data.get('id_producto')
    precio = data.get('precio', 0)
    numero_productos = data.get('numero_productos', 0)
    total = data.get('total', 0)
    
    # Lógica simulada: Valida que el precio por el número de productos sea igual al total cobrado
    # Retorna true si es exitosa, false en caso contrario [cite: 24, 35, 37]
    compra_exitosa = (precio * numero_productos) == total
    
    print(f"Procesando compra del producto {id_producto}...")
    return jsonify({'exito': compra_exitosa})

if __name__ == '__main__':
    # Inicia el servidor en el puerto 8080
    app.run(port=8080)
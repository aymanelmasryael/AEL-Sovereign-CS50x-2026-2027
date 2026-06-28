from flask import Flask, jsonify, request

# Standard CS50 Flask web application demonstration

app = Flask(__name__)

@app.route("/")
def index():
    return jsonify({"service": "CS50 Finance API", "status": "active", "version": "1.0.0"})

@app.route("/quote", methods=["GET"])
def quote():
    symbol = request.args.get("symbol", "NFLX")
    return jsonify({"symbol": symbol, "price": 645.25, "currency": "USD"})

if __name__ == "__main__":
    app.run(port=5000, debug=True)

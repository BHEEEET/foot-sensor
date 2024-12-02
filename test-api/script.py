from flask import Flask, request

app = Flask(__name__)

@app.route('/test', methods=['POST'])
def test_api():
    data = request.json
    print("Received data:", data)
    return {"message": "Data received successfully"}, 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8080)


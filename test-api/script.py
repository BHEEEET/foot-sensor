from flask import Flask, request, jsonify
from pymongo import MongoClient
import os

app = Flask(__name__)

# MongoDB Configuration
MONGO_URI = os.getenv("MONGO_URI", "mongodb://localhost:27017/")
client = MongoClient(MONGO_URI)
db = client["foot-sensor"]  # Replace with your database name
collection = db["sensor"]  # Replace with your collection name

@app.route('/api/data', methods=['POST'])
def save_data():
    data = request.json
    if not data:
        return jsonify({"error": "Invalid or missing JSON data"}), 400

    collection.insert_one(data)
    return jsonify({"message": "Data saved successfully"}), 200
    
@app.route('/api/data', methods=['GET'])
def get_data():
    documents = list(collection.find({}, {"_id": 0}))
    return jsonify(documents), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8080)
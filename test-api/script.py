from flask import Flask, request, jsonify
from pymongo import MongoClient
import os

app = Flask(__name__)

# MongoDB Configuration
MONGO_URI = os.getenv("MONGO_URI", "mongodb://localhost:27017/")
client = MongoClient(MONGO_URI)
db = client["foot-sensor"]  # Replace with your database name
collection = db["sensor"]  # Replace with your collection name

@app.route('/test', methods=['POST'])
def test_api():
    data = request.json  # Parse the incoming JSON data
    if not data:
        return jsonify({"error": "Invalid or missing JSON data"}), 400

    try:
        # Insert data into MongoDB
        result = collection.insert_one(data)
        print(f"Inserted document ID: {result.inserted_id}")
        return jsonify({"message": "Data received and stored successfully"}), 200
    except Exception as e:
        print(f"Error inserting data: {e}")
        return jsonify({"error": "Failed to store data"}), 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8080)
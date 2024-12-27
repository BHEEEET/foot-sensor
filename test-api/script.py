from flask import Flask, request, jsonify
from flask_cors import CORS
from pymongo import MongoClient
import os
import datetime 

app = Flask(__name__)

# Enable CORS
CORS(app, resources={r"/api/*": {"origins": "http://127.0.0.1:4200"}})

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
    
    time_stamp = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    data["timestamp"] = time_stamp

    collection.insert_one(data)
    return jsonify({"message": "Data saved successfully"}), 200
    
@app.route('/api/data', methods=['GET'])
def get_data():
    documents = list(collection.find({}, {"_id": 0}))
    return jsonify(documents), 200

@app.route('/api/data/count', methods=['GET'])
def get_data_count():
    # Aggregation query to count occurrences of sensor_value per day
    pipeline = [
        {
            "$project": {
                "date": {
                    "$dateToString": {
                        "format": "%Y-%m-%d",
                        "date": {
                            "$toDate": "$timestamp"  # Convert to date if it's not already in Date format
                        }
                    }
                },
                "sensor_value": 1
            }
        },
        {
            "$group": {
                "_id": "$date",
                "count": {"$sum": 1}
            }
        },
        {
            "$sort": {"_id": 1}  # Sort by date (ascending)
        }
    ]
    
    # Perform aggregation query
    result = list(collection.aggregate(pipeline))
    
    return jsonify(result), 200


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8080)
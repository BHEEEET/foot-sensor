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
sensor_collection = db["sensor"]  # Replace with your collection name
reward_collection = db["rewards"]  # Replace with your collection name

@app.route('/api/data', methods=['POST'])
def save_data():
    data = request.json
    
    if not data:
        return jsonify({"error": "Invalid or missing JSON data"}), 400
    
    sensor_collection.insert_one(data)
    return jsonify({"message": "Data saved successfully"}), 200

    
@app.route('/api/data', methods=['GET'])
def get_data():
    documents = list(sensor_collection.find({}, {"_id": 0}))
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
    result = list(sensor_collection.aggregate(pipeline))
    
    return jsonify(result), 200

@app.route('/api/reward', methods=['GET'])
def get_rewards():
    # Fetch all reward records
    rewards = list(reward_collection.find({}, {"_id": 0}))
    return jsonify(rewards), 200


@app.route('/api/reward', methods=['POST'])
def handle_reward():
    data = request.json
    
    if not data:
        return jsonify({"error": "Invalid or missing reward data"}), 400
    
    reward_collection.insert_one(data)
    
    return jsonify({"message": "Reward granted successfully"}), 200

@app.route('/api/reward/total', methods=['GET'])
def get_total_rewards():
    # Aggregate total points per user
    pipeline = [
        {
            "$group": {
                "_id": "$user",
                "total_points": {"$sum": "$points"}
            }
        },
        {
            "$project": {
                "user": "$_id",
                "total_points": 1,
                "_id": 0
            }
        }
    ]

    totals = list(reward_collection.aggregate(pipeline))

    return jsonify(totals), 200

@app.route('/api/reward/total-per-day', methods=['GET'])
def get_total_rewards_per_day():
    # Aggregate total points per day with users listed under each date
    pipeline = [
        {
            "$group": {
                "_id": {
                    "user": "$user",
                    "date": {"$substr": ["$timestamp", 0, 10]}
                },
                "total_points": {"$sum": "$points"}
            }
        },
        {
            "$group": {
                "_id": "$_id.date",
                "users": {
                    "$push": {
                        "user": "$_id.user",
                        "total_points": "$total_points"
                    }
                }
            }
        },
        {
            "$project": {
                "date": "$_id",
                "users": 1,
                "_id": 0
            }
        },
        {
            "$sort": {"date": 1}
        }
    ]

    totals_per_day = list(reward_collection.aggregate(pipeline))

    return jsonify(totals_per_day), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8080)
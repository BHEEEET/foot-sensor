from flask import Flask, request, jsonify, render_template_string
from datetime import datetime

app = Flask(__name__)

data_list = []

# HTML template to display data with auto-refresh
html_template = '''
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Sensor Data</title>
    <style>
        body { font-family: Arial, sans-serif; display: flex; justify-content: center; align-items: center; height: 100vh; background-color: #f4f4f4; margin: 0; }
        #data-container { text-align: center; background-color: #ffffff; padding: 20px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); border-radius: 8px; }
        .data-item { margin: 10px 0; font-size: 1.2em; }
    </style>
    <script>
        function refreshPage() {
            setInterval(function() {
                window.location.reload();
            }, 1000);  // Refresh every 1 seconds
        }
    </script>
</head>
<body onload="refreshPage()">
    <div id="data-container">
        <h1>Sensor Data</h1>
        {% for item in data_list %}
        <div class="data-item">{{ item }}</div>
        {% endfor %}
    </div>
</body>
</html>
'''

# API endpoint to receive data
@app.route('/api/data', methods=['POST'])
def receive_data():
    data = request.json
    timestamp = data.get('timestamp')
    if timestamp:
        formatted_timestamp = datetime.fromisoformat(timestamp).strftime('%Y-%m-%d %H:%M:%S')
        data_list.insert(0, formatted_timestamp)  # Insert new formatted data at the top
        if len(data_list) > 10:  # Keep only the last 10 items
            data_list.pop()  # Remove the oldest item
    print(f"Received data: {data}")
    return jsonify({"status": "success"}), 200

# HTML page to display the data
@app.route('/')
def display_data():
    return render_template_string(html_template, data_list=data_list)

if __name__ == '__main__':
    app.run(port=8080, debug=True)

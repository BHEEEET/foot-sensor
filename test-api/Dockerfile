# Use Python as base image
FROM python:3.10-slim

# Set the working directory
WORKDIR /api

# Copy and install requirements
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

# Copy the application code
COPY . .

# Expose port 5000 and run the application
EXPOSE 5000
CMD ["python", "script.py"]

# Use a lightweight Node.js base image
FROM node:21-alpine

# Set the working directory in the container
WORKDIR /app

# Install Angular CLI globally
RUN npm install -g @angular/cli@18.0.0

# Disable Google Analytics Pop-up
ENV NG_CLI_ANALYTICS false

# Copy only package.json and package-lock.json first for better caching
COPY package*.json ./

# Install dependencies
RUN npm install

# Copy the rest of the application code
COPY . .

# Expose port 4200 for the Angular development server
EXPOSE 4200

# Start the Angular development server
CMD ["ng", "serve", "--host", "0.0.0.0"]

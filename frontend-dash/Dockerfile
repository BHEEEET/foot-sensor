# Use Node.js as base image
FROM node:16-alpine

# Set the working directory
WORKDIR /app

# Copy package.json and install dependencies
COPY package*.json ./
RUN npm install

# Copy the application code
COPY . .

# Expose port 4200 and start the Angular development server
EXPOSE 4200
# CMD ["npm", "start"]

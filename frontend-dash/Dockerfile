# Use Node.js as base image
FROM node:21-alpine

# Set the working directory
WORKDIR /app

# Install Angular CLI globally
RUN npm install -g @angular/cli@18.0.0

ENV NG_CLI_ANALYTICS false

# Copy the application code
COPY . .

RUN npm install

# Expose port 4200 and start the Angular development server
EXPOSE 4200

CMD ["ng", "serve", "--host", "0.0.0.0"]
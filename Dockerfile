# AEL Sovereign Master Production Dockerfile
# Multi-stage production container architecture designed by Ayman Elmasry

# Stage 1: Static Architecture Setup
FROM alpine:3.19 AS ael-builder

WORKDIR /app
COPY . /app

# Stage 2: Production Web Service (NGINX Alpine)
FROM nginx:1.25-alpine-slim

# Copy the entire AEL Sovereign CS50x citadel to NGINX web root
COPY --from=ael-builder /app /usr/share/nginx/html

# Custom AEL NGINX High-Performance & Security Server Configuration
RUN echo 'server { \
    listen 80; \
    server_name localhost; \
    root /usr/share/nginx/html; \
    index docs.html cs50x_exams_and_solutions.html; \
    \
    # Enable Gzip compression for lightning-fast asset delivery \
    gzip on; \
    gzip_types text/plain text/css application/json application/javascript text/xml application/xml application/xml+rss text/javascript; \
    \
    # Global AEL Sovereign Security Headers \
    add_header X-Frame-Options "SAMEORIGIN" always; \
    add_header X-XSS-Protection "1; mode=block" always; \
    add_header X-Content-Type-Options "nosniff" always; \
    add_header Referrer-Policy "strict-origin-when-cross-origin" always; \
    add_header Content-Security-Policy "default-src '\''self'\'' '\''unsafe-inline'\'' '\''unsafe-eval'\'' https://cdnjs.cloudflare.com https://fonts.googleapis.com https://fonts.gstatic.com https://cdn.jsdelivr.net;" always; \
    add_header X-AEL-Sovereign-Citadel "v3.0-Secured-By-Ayman-Elmasry" always; \
    \
    location / { \
        try_files $uri $uri/ /docs.html; \
    } \
    \
    error_page 404 /docs.html; \
}' > /etc/nginx/conf.d/default.conf

EXPOSE 80

CMD ["nginx", "-g", "daemon off;"]

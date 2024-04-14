function login_user {
  curl \
    -s \
    -k \
    -X POST \
    'https://localhost/api/login_user?email=ssleert@gmail.com&password=swagonme' \
  | msgpack-cli decode \
  | jq -r .access_token 
}

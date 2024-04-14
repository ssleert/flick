curl \
  -s \
  -k \
  -X POST \
  'https://localhost/api/register_user?email=ssleert@gmail.com&username=ssleert&password=swagonme' \
  | msgpack-cli decode


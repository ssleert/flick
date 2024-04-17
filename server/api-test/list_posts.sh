source "./login_user.sh"

function create_post {
  local access_token=$(login_user)

  curl \
    -s \
    -k \
    -H "auth-token: ${access_token}" \
    'https://localhost/api/list_posts?offset=0&limit=100' \
  | msgpack-cli decode --pp
}

create_post

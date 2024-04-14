source "./login_user.sh"

function create_post {
  local access_token=$(login_user)

  local json_body='{
    "body": "test post",
    "attachments": ["123", "123"],
    "is_comments_disallowed": true
  }'

  curl \
    -s \
    -k \
    -d "$(printf "${json_body}" | msgpack-cli encode)" \
    -H "auth-token: ${access_token}" \
    'https://localhost/api/create_post' \
    | msgpack-cli decode --pp
}

create_post

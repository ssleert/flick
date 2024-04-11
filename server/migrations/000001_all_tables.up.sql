CREATE TABLE IF NOT EXISTS users (
  user_id       SERIAL       PRIMARY KEY,
  email         VARCHAR(256) UNIQUE NOT NULL,
  password_hash VARCHAR(256) NOT NULL,
  nickname      VARCHAR(256) UNIQUE NOT NULL,
  avatar_img    VARCHAR(100) NOT NULL,
  creation_date BIGINT       NOT NULL
);

CREATE TABLE IF NOT EXISTS users_tokens (
  token_id      SERIAL       PRIMARY KEY,
  user_id       INTEGER      NOT NULL REFERENCES users(user_id),
  refresh_token VARCHAR(256) UNIQUE NOT NULL,
  ip_addr       VARCHAR(256) NOT NULL,
  user_agent    VARCHAR(256) NOT NULL,
  token_date    BIGINT       NOT NULL
)

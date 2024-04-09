CREATE TABLE IF NOT EXISTS users (
  user_id       SERIAL       PRIMARY KEY,
  email         VARCHAR(256) UNIQUE NOT NULL,
  password_hash VARCHAR(256) NOT NULL,
  nickname      VARCHAR(256) UNIQUE NOT NULL,
  avatar_img    VARCHAR(100) NOT NULL,
  creation_date BIGINT       NOT NULL
);

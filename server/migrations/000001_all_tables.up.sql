CREATE TABLE IF NOT EXISTS users (
  user_id       SERIAL PRIMARY KEY,
  email         TEXT   UNIQUE NOT NULL,
  password_hash TEXT   NOT NULL,
  nickname      TEXT   UNIQUE NOT NULL,
  avatar_img    TEXT   NOT NULL,
  creation_date BIGINT NOT NULL
);

CREATE TABLE IF NOT EXISTS users_tokens (
  token_id      SERIAL  PRIMARY KEY,
  user_id       INTEGER NOT NULL REFERENCES users(user_id),
  refresh_token TEXT    UNIQUE NOT NULL,
  ip_addr       TEXT    NOT NULL,
  user_agent    TEXT    NOT NULL,
  token_date    BIGINT  NOT NULL
);

CREATE TABLE IF NOT EXISTS users_posts (
  post_id                SERIAL      PRIMARY KEY,
  user_id                INTEGER     NOT NULL REFERENCES users(user_id),
  creation_date          BIGINT      NOT NULL,
  body                   TEXT        NOT NULL,
  attachments            TEXT[]      NOT NULL,
  likes_amount           INTEGER     NOT NULL,
  dislikes_amount        INTEGER     NOT NULL,
  comments_amount        INTEGER     NOT NULL,
  is_comments_disallowed BOOLEAN     NOT NULL
);

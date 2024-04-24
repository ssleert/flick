import { Link } from "hywer/x/router";

import css from "./NavBar.module.less";

const NavBar = () => {
  return <div class={css.NavBar}>
    <Link path="/" class={css.Route}>
      P
    </Link>
    <Link path="/messages" class={css.Route}>
      M
    </Link>
    <Link path="/new" class={css.Route}>
      N
    </Link>
    <Link path="/search" class={css.Route}>
      S
    </Link>
    <Link path="/settings" class={css.Route}>
      St
    </Link>
  </div>
}

export default NavBar;

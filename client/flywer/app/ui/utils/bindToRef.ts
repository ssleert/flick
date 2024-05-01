import { type Reactive } from "hywer";

// TODO: add to hywer/x/html
const bindToRef = (refObj: Reactive<string>) => (e: InputEvent) =>
  (refObj.val = (e.target as HTMLInputElement).value);

export default bindToRef;

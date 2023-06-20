import { Daily } from "./frequencies.js";

export const formTypeHandler = (type) => {
  if (type === Daily)
    document.getElementsByClassName("day-input")[0].style.display = "none";
  else {
    document.getElementsByClassName("day-input")[0].style.display = "inline";
  }
};

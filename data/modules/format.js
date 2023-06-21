import { Daily, Weekly } from "./frequencies.js";

export const formatSchedule = (scheduleStr) => {
  const arr = scheduleStr.split(";");
  const filtered = arr.filter((a) => a != "");

  return filtered;
};

export const formatScheduleObj = (scheduleStr) => {
  let scheduleObj = {
    type: null,
    duration: null,
    hour: null,
    day: null,
  };

  const splitted = scheduleStr.split(" ");
  const formatted = splitted.filter((w) => w != "");

  if (scheduleStr.includes(Daily)) {
    scheduleObj.type = formatted[0];
    scheduleObj.duration = formatted[1];
    scheduleObj.hour = formatted[2];
  } else if (scheduleStr.includes(Weekly)) {
    scheduleObj.type = formatted[0];
    scheduleObj.day = formatted[1];
    scheduleObj.duration = formatted[2];
    scheduleObj.hour = formatted[3];
  }

  return scheduleObj;
};

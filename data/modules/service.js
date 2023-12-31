import { Daily } from "./enums.js";

export const getSchedule = async () => {
  try {
    const result = await fetch("/schedule", {
      method: "GET",
    });

    const parsed = await result.json();
    return parsed?.schedule;
  } catch (err) {
    console.error(err);
    return "";
  }
};

export const addSchedule = async (type, day, hour, duration) => {
  const scheduleStr =
    type === Daily
      ? `${type} ${duration} ${hour};`
      : `${type} ${day} ${duration} ${hour};`;
  try {
    const result = await fetch(`/schedule?schedule=${scheduleStr}`, {
      method: "POST",
    });

    if (result.status === 200) return true;
    return false;
  } catch (err) {
    console.error(err);
    return false;
  }
};

export const removeSchedule = async (scheduleStr) => {
  try {
    const result = await fetch(`/schedule?schedule=${scheduleStr};`, {
      method: "DELETE",
    });

    if (result.status === 200) return true;
    return false;
  } catch (err) {
    console.error(err);
    return false;
  }
};

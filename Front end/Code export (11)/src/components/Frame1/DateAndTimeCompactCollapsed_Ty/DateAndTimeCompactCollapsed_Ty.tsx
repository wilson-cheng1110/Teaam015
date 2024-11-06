import { memo } from 'react';
import type { FC } from 'react';

import resets from '../../_resets.module.css';
import classes from './DateAndTimeCompactCollapsed_Ty.module.css';

interface Props {
  className?: string;
  classes?: {
    root?: string;
  };
}
/* @figmaId 155:77 */
export const DateAndTimeCompactCollapsed_Ty: FC<Props> = memo(function DateAndTimeCompactCollapsed_Ty(props = {}) {
  return (
    <div className={`${resets.clapyResets} ${props.classes?.root || ''} ${props.className || ''} ${classes.root}`}>
      <div className={classes.date}>
        <div className={classes.month}>Jun 10,</div>
        <div className={classes.year}>2024</div>
      </div>
      <div className={classes.time}>
        <div className={classes.time2}>9:41 AM</div>
      </div>
    </div>
  );
});

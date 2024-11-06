import { memo } from 'react';
import type { FC } from 'react';

import resets from '../../_resets.module.css';
import classes from './LinearIndeterminateProgressInd.module.css';

interface Props {
  className?: string;
  classes?: {
    root?: string;
  };
}
/* @figmaId 175:178 */
export const LinearIndeterminateProgressInd: FC<Props> = memo(function LinearIndeterminateProgressInd(props = {}) {
  return (
    <div className={`${resets.clapyResets} ${props.classes?.root || ''} ${props.className || ''} ${classes.root}`}>
      <div className={classes.track}>
        <div className={classes.trackShape}></div>
      </div>
    </div>
  );
});

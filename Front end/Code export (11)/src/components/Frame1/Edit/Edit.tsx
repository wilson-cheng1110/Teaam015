import { memo } from 'react';
import type { FC, ReactNode } from 'react';

import resets from '../../_resets.module.css';
import classes from './Edit.module.css';
import { EditIcon } from './EditIcon.js';

interface Props {
  className?: string;
  swap?: {
    icon?: ReactNode;
  };
}
/* @figmaId 158:781 */
export const Edit: FC<Props> = memo(function Edit(props = {}) {
  return (
    <div className={`${resets.clapyResets} ${classes.root}`}>
      <div className={classes.icon}>{props.swap?.icon || <EditIcon className={classes.icon2} />}</div>
    </div>
  );
});

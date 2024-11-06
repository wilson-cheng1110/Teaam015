import { memo } from 'react';
import type { FC } from 'react';

import resets from '../../_resets.module.css';
import { Check } from '../Check/Check.js';
import classes from './BuildingBlocksSegmentedButtonB.module.css';
import { SelectedIconIcon } from './SelectedIconIcon.js';

interface Props {
  className?: string;
  classes?: {
    container?: string;
    root?: string;
  };
  hide?: {
    stateLayer?: boolean;
  };
}
/* @figmaId 158:1166 */
export const BuildingBlocksSegmentedButtonB: FC<Props> = memo(function BuildingBlocksSegmentedButtonB(props = {}) {
  return (
    <div className={`${resets.clapyResets} ${props.classes?.root || ''} ${props.className || ''} ${classes.root}`}>
      <div className={`${props.classes?.container || ''} ${classes.container}`}>
        {!props.hide?.stateLayer && (
          <div className={classes.stateLayer}>
            <Check
              className={classes.selectedIcon}
              swap={{
                icon: <SelectedIconIcon className={classes.icon} />,
              }}
            />
            <div className={classes.labelText}>Label</div>
          </div>
        )}
      </div>
    </div>
  );
});

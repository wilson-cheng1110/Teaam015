import { memo, SVGProps } from 'react';

const Rectangle2Icon = (props: SVGProps<SVGSVGElement>) => (
  <svg preserveAspectRatio='none' viewBox='0 0 855 1669' fill='none' xmlns='http://www.w3.org/2000/svg' {...props}>
    <path
      d='M0 40C0 17.9086 17.9086 0 40 0H814.46C836.551 0 854.46 17.9086 854.46 40V1629C854.46 1651.09 836.551 1669 814.46 1669H39.9999C17.9086 1669 0 1651.09 0 1629V40Z'
      fill='white'
    />
  </svg>
);

const Memo = memo(Rectangle2Icon);
export { Memo as Rectangle2Icon };
